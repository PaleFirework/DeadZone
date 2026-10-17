# DeadZone

Unreal Engine 5.7 项目。本仓库**仅包含源代码与配置文件**，`Content/` 下的游戏素材**未包含在内**（详见[素材说明](#素材说明)）。

## 环境要求

- Unreal Engine **5.7**
- 已启用插件（见 `DeadZone.uproject`）：
  - GameplayAbilities
  - CommonUI
  - ModelingToolsEditorMode（仅编辑器）
  - ThumbnailExportManager（仅编辑器）

## 仓库内容

| 路径 | 说明 |
| --- | --- |
| `Source/` | C++ 游戏模块（`DeadZone`）源码，共 116 个文件 |
| `Config/` | 工程配置（引擎 / 编辑器 / 游戏 INI） |
| `DeadZone.uproject` | 工程描述文件（引擎版本关联、插件列表） |
| `DeadZone.sln` | Visual Studio 解决方案 |
| `DeadZone.Target.cs` / `DeadZoneEditor.Target.cs` | 构建目标 |

## 使用方式

1. 克隆本仓库。
2. 用 Unreal Engine 5.7 打开 `DeadZone.uproject`，弹出提示时允许重新编译 `DeadZone` C++ 模块。
3. 将所需的商城素材包装入 `Content/`（素材就位前，工程会提示资源缺失）。

## 素材说明

所有美术 / 音频 / 蓝图资源位于 `Content/`，通过 Unreal Engine 商城分发，**不随本仓库提供**。工程使用到的素材包包括（Synty Studios 及第三方 GUI / 动画套件）：

- POLYGON Military / Apocalypse / City Zombies / Heist / Battle Royale
- 僵尸角色与动画套件
- Horror grunge GUI 素材包

再分发任何衍生内容前，请查阅各素材包的授权条款。

## 许可证

本仓库中的源代码按“原样”提供。商城素材遵循其各自的 Unreal Engine Marketplace 授权，不属于本仓库授权范围。
