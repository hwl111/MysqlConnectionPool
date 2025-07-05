# MySQL Connection Pool in C++

这是一个使用 C++ 实现的 MySQL 数据库连接池项目，适用于需要高效复用数据库连接的高并发应用场景。

## 🌟 项目特点

- ✅ **自定义连接池实现**：支持最大连接数限制、空闲连接重用、自动初始化与资源释放
- 🚀 **线程安全的连接管理**：使用互斥锁或其他同步机制，确保多线程环境下的连接安全
- 🛠️ **空闲连接维护机制**：通过内置机制定期清理长期未使用的连接，提升资源利用率
- 📦 **支持配置文件初始化**：通过 `mysql.ini` 可灵活设置数据库地址、用户名、连接数等参数
- 🧩 **高可拓展性**：连接池结构设计清晰，便于扩展为异步连接、超时重试等高级功能

## 🗂️ 项目结构

```
MysqlConnectionPool/
├── Connection.cpp / .h           # 封装 MySQL 连接的类
├── MysqlConnectionPool.cpp / .h # 连接池的核心实现
├── main.cpp                      # 测试或演示程序入口
├── mysql.ini                     # 数据库配置文件
├── libmysql.dll                 # MySQL 客户端动态链接库（Windows 环境）
├── public.h                     # 公共头文件（可能用于宏或通用声明）
└── *.vcxproj / .sln             # Visual Studio 工程文件
```

## ⚙️ 配置文件示例（mysql.ini）

```ini
host = 127.0.0.1
port = 3306
user = root
password = yourpassword
database = testdb
max_connections = 10
```

请根据你的数据库实际配置修改 `mysql.ini` 文件。

## 🔧 编译与运行

### 要求

- Visual Studio 2019 或更高版本
- 安装 MySQL C API（已随项目提供 `libmysql.dll`）

### 步骤

1. 打开 `MysqlConnectionPool.sln`
2. 使用 Release 或 Debug 模式编译项目
3. 将 `mysql.ini` 放在可执行文件相同目录下
4. 运行程序查看连接池运行效果

## 🧪 示例输出

![性能对比](performance_comparison.png)

✅ 可见连接池显著降低了数据库访问延迟，尤其在多线程场景下提升尤为明显。

## 📄 License

此项目仅用于学习与研究目的。请在遵循 MySQL 相关授权协议下使用。

---

💡 如果你喜欢这个项目，欢迎点个 ⭐ Star！
