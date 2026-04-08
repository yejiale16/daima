# SnakeGame (Console + CMake)

## 项目简介
基于 C++ 的控制台贪吃蛇工程化实现，包含：
- 双缓冲渲染（降低闪烁）
- 键盘输入控制（方向、暂停、退出）
- 蛇身数据结构管理（`std::deque<Point>`）
- 碰撞检测（墙体/自身）
- 分数系统
- 存档/读档（文本序列化）
- 游戏状态管理（Start / Running / Paused / GameOver）

## 目录结构
- `main.cpp`
- `Game.h` / `Game.cpp`
- `Snake.h` / `Snake.cpp`
- `Food.h` / `Food.cpp`
- `Renderer.h` / `Renderer.cpp`
- `Input.h` / `Input.cpp`
- `SaveSystem.h` / `SaveSystem.cpp`
- `CMakeLists.txt`

## 编译运行（Windows + VS2022）
```powershell
cmake -S . -B build
cmake --build build
.\build\Debug\SnakeGame.exe
```

## 操作说明
- `W/A/S/D` 或方向键：移动
- `SPACE`：开始/重新开始
- `P`：暂停/继续
- `K`：存档
- `L`：读档
- `Q` 或 `Esc`：退出

## 存档格式（savegame.txt）
```txt
score=10
direction=1
status=2
food=10,10
snake=5,5;5,6;5,7
```

## AI 辅助开发说明（可用于作业报告）
- 工具：GitHub Copilot（GPT-5.3-Codex）
- 参与内容：模块化代码框架、存档系统、输入与渲染逻辑、编译问题修复
- 人工修改：按终端窗口自适应游戏区域、修复 Windows 头文件宏冲突
- 调试记录：处理 CMake Tools 配置失败，改用命令行 CMake 完成构建验证
