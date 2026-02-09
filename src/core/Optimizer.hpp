#pragma once

#include "Logger.hpp"
#include "../ui/StateContext.hpp"
#include "./models/AppStats.hpp"



namespace Reducord::Core::Optimizer
{
	enum class TaskType
	{
		CleanCache,
		CleanLogs,
		CleanVersions,
		HigherPriorityProcess
	};

	class ITask
	{
	public:
		virtual ~ITask() = default;
		virtual void Execute(Models::AppStats &state, Logger::ILogger &logger) = 0;
		virtual std::string GetName() const = 0;
	};

	class TaskFactory
	{
	public:
		static std::unique_ptr<ITask> CreateTask(TaskType type);
	};


	class TaskManager
	{
	public:
		static void RunQueue(Models::AppStats &state,
							 Logger::ILogger& logger,
							 const std::vector<TaskType> &tasks
							);
	public:
		static void RunQueue(Models::AppStats &state,
							 Logger::ILogger& logger,
							 const std::vector<TaskType> &tasks,
							 std::function<void()> callback
							);
	};

	class TaskRunner {

        private: std::vector<TaskType> _tasks;
		private: bool _inProcess;

        public: static TaskRunner& GetOrCreate() {
            static TaskRunner r;
            return r;
        }

        public: TaskRunner& AddTask(TaskType t) {
			return AddTaskConditional(t, true);
        }

		public: TaskRunner& AddTaskConditional(TaskType t, bool c) {
			if (_inProcess) return *this;
			if (c) _tasks.push_back(t);
            return *this;
        }

		public: TaskRunner& FlushEx(Reducord::UI::StateContext& ctx, bool async) {
			if (_inProcess) {
				ctx.logger->Error("Optimizing is in process. Please wait.");
				return *this;
			}
			ctx.logger->Info("[Running tasks via TaskRunner by KCJ]");

			Reducord::UI::StateContext* ctx_ = &ctx;

			if (_tasks.empty()) {
				ctx.logger->Warning("Nothing to do. Please select at least one");
				return *this;
			}

			auto callback = [this](){
				_inProcess = false;
				_tasks.clear();
			};

			_inProcess = true;

			std::thread worker([this, ctx_, callback]() {
				TaskManager::RunQueue(ctx_->GetStatsMutable(), *ctx_->logger, _tasks, callback);
			});

			if (async) worker.detach();
			else worker.join();

			return *this;
		}

		public: TaskRunner& Flush(Reducord::UI::StateContext& ctx) {
			return FlushEx(ctx, true);
		}

        private: TaskRunner(){}
        private: ~TaskRunner(){}

        private: TaskRunner(TaskRunner const&) = delete;
        private: TaskRunner& operator= (TaskRunner const&) = delete;
    };

	namespace Utils
	{
		bool IsDiscordRunning();
		std::string GetDiscordPath();
		std::string FormattedSize(uintmax_t bytes);
	}
}