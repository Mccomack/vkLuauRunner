export module luauHelper;
import luau;
import std;

export namespace luauHelper {
    luau::Environment getEnv(const luau::SecurityType sec) {
        static std::unordered_map<luau::SecurityType, luau::State> states;
        auto [it, inserted] = states.try_emplace(sec, sec);
        if (inserted)
            luau::sandbox(it->second);

        auto env = luau::Environment(it->second);
        luau::sandbox(env);

        return env;
    }

    void registerAll(const luau::Environment& env) {
        luau::global::registerDefaultFunctions(*env);
        luau::library::registerDefaultLibraries(*env);
        luau::object::registerDefaultObjects(*env);
    }
}
