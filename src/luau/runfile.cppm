module;
#include <lua.h>
#include <lualib.h>
#include <luacode.h>
#include <luacodegen.h>
#include <Luau/Compiler.h>
#include <Luau/CodeGen.h>

export module luau:runfile;
import std;

export namespace luau::run {
    std::string readFile(const std::filesystem::path& path);
    void runLuauFile(lua_State* L, const std::filesystem::path& path);
}

std::string luau::run::readFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("cannot open file: " + path.string());
    
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void luau::run::runLuauFile(lua_State* L, const std::filesystem::path& path) {
    const std::string source   = readFile(path);
    const std::string chunkName = "@" + path.filename().string();

    // 1. 컴파일 (optimizationLevel 2 권장 - codegen과 함께 쓸 때)
    Luau::CompileOptions opts{
        .optimizationLevel = 2
    };

    const std::string bytecode = Luau::compile(source, opts);

    // 2. VM에 로드
    if (luau_load(L, chunkName.c_str(), bytecode.data(), bytecode.size(), 0) != LUA_OK) {
        const std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw std::runtime_error("luau_load failed: " + err);
    }

    // 3. CodeGen 적용 (luau_load 직후, pcall 전에)
    if (luau_codegen_supported()) {
        Luau::CodeGen::CompilationOptions nativeOpts{};
        // CodeGen_OnlyNativeModules: --!native 또는 @native 붙은 함수만 네이티브 컴파일
        // 플래그 없이 두면: 청크 전체를 무조건 네이티브 컴파일
        nativeOpts.flags = Luau::CodeGen::CodeGenFlags::CodeGen_OnlyNativeModules;

        Luau::CodeGen::CompilationResult result = Luau::CodeGen::compile(L, -1, nativeOpts);
        // 실패해도 fatal은 아님 - 그냥 인터프리터로 실행됨
    }

    // 4. 실행
    if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
        const std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw std::runtime_error("lua_pcall failed: " + err);
    }
}