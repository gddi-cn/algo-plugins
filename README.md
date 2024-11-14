# 共达地算法后处理插件

## 开发环境设置

本项目支持使用 Visual Studio Code 的 Dev Containers 功能来快速设置开发环境。请按照以下步骤进行操作：

1. **安装必要的扩展**:
   - 确保您已安装 [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) 扩展。

2. **打开项目**:
   - 在 Visual Studio Code 中打开本项目的根目录。

3. **重载窗口**:
   - 按下 `F1`，输入并选择 `Remote-Containers: Reopen in Container`。这将根据 `.devcontainer` 文件夹中的配置构建并启动容器。

4. **开始开发**:
   - 一旦容器启动，您就可以在容器内进行开发，所有依赖和工具都已预先配置。

### 注意事项
- 确保您的 Docker 已正确安装并运行，因为 Dev Containers 依赖于 Docker。
- 如果您需要自定义开发环境，可以编辑 `.devcontainer/devcontainer.json` 文件。


## 接口文档

### 1. `AlgoObject` 结构体
- **描述**: 表示算法对象的基本信息。
- **成员**:
  - `int target_id`: 目标 ID。
  - `std::string label`: 标签。
  - `float score`: 分数。
  - `std::vector<int> rect`: 矩形区域。
  - `int64_t track_id`: 跟踪 ID。
  - `std::vector<Keypoint> keypoints`: 目标关键点。
  - `std::string ocr_text`: OCR 文本。
  - `std::vector<float> feature`: 目标特征。
  - `std::vector<uint8_t> segment`: 分割图。
  - `std::vector<std::vector<int>> contours`: 轮廓。

### 2. `PropertyEntry` 结构体
- **描述**: 表示插件属性的条目。
- **成员**:
  - `nlohmann::json local_value`: 本地值。
  - `std::function<void(nlohmann::json &&value)> setter`: 设置器函数。
  - `std::function<nlohmann::json()> getter`: 获取器函数。

### 3. `AbstractPlugin` 类
- **描述**: 插件的抽象基类，定义了插件的基本接口。
- **构造函数**:
  - `AbstractPlugin(std::string name)`: 构造函数，接受插件名称。
  
- **虚函数**:
  - `virtual std::vector<AlgoObject> infer_result_process(const std::vector<AlgoObject> &objects)`: 处理推理结果，返回处理后的对象列表。
  - `virtual std::vector<AlgoObject> tracked_result_process(const std::vector<AlgoObject> &objects)`: 处理跟踪结果，返回处理后的对象列表。
  - `virtual std::string get_plugin_name() const`: 获取插件名称。
  - `virtual nlohmann::json get_plugin_definition()`: 获取插件定义。
  
- **成员函数**:
  - `nlohmann::json get_properties() const`: 获取插件参数。
  - `bool try_set_property(const std::string &key, nlohmann::json value)`: 尝试设置插件参数。

- **保护成员函数**:
  - `template<typename T> void bind_property(const std::string &name, T &value, const std::string &description)`: 绑定属性。
  - `PropertyEntry &add_property(const std::string &key, const std::function<void()> &on_changed)`: 添加属性。

- **成员变量**:
  - `std::string plugin_name_`: 插件名称。
  - `std::map<std::string, PropertyEntry> properties_`: 插件属性集合。