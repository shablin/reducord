<p align="center">
  <img src="assets/logo.png" alt="Reducord Logo" width="256">
  <br>
  <b>A lightweight, high-performance cleanup utility for Discord power users</b>
  <br>
  <i>Built with C++, Dear ImGui, and DirectX 11</i>
</p>

<p align="center">
  <img src="https://img.shields.io/github/v/release/shablin/reducord?style=for-the-badge&color=blue" alt="Release">
  <img src="https://img.shields.io/github/license/shablin/reducord?style=for-the-badge" alt="License">
  <img src="https://img.shields.io/github/actions/workflow/status/shablin/reducord/cmake-single-platform.yml?style=for-the-badge" alt="Build Status">
</p>


## 🚀 Key Features

* **🧹 Deep Cache Cleaning:** Safely wipe GPU cache, IndexDB, and temporary files that slow down the app.
* **🧼 logs Log Eraser:** Clear out bloated log files that eat up your disk space.
* **📦 Version Manager:** Identify and remove old Discord versions left behind after updates.
* **🔒 Safety First:** Built-in protection prevents cleaning while Discord is running to avoid data corruption.

## 📸 Screenshots

<p align="center">
  <img src="assets/screenshot.PNG" alt="App Screenshot" width="400">
</p>


## 🛠️ Tech Stack

- **Language:** C++17
- **GUI:** [Dear ImGui](https://github.com/ocornut/imgui)
- **Graphics:** DirectX 11
- **Architectural Pattern:** Clean Architecture (Core/UI decoupling)
- **Dependency Management:** [vcpkg](https://github.com/microsoft/vcpkg)


## 🏗️ Building from Source

Building guide is available at [Wiki](https://github.com/shablin/reducord/wiki/Building).