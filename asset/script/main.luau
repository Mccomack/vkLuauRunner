print(
    add(1, 2),
    "asf"
)

print(security())
print("im not gonna die")

printSecurityLevel()

print("adadfafs")

print(
    xpcall(
        function(v1, v2)
            assert(add(v1, v2) == -1, "asdf")
        end, 
        function(...)
            print(...)
            return true
        end, 
        2, 3
    )
)

print({
    asdf = "asdf";
    ["aagdf"] = "asdfd"
})

print()
print(nil)

print(test.somemethod());
print(getmetatable(test))
print(test)
print(table)

print("나다 이 잡것들아. ")
print(test.toMoneyFace("나다 이 잡것들아. "))
-- print(test.toMoneyFace())
print(test.toMoneyFace(nil))
print(test.toMoneyFace(12134))

local function printTable(t: {[any]: any}, skipname: boolean, name: string, _tabs: number?)
    local tabs = _tabs or 1

    local _str = ""

    for _ = 0, tabs - 1, 1 do
        _str ..= "\t"
    end

    print(`{_str}{if not skipname then `{name}: ` else ""}\{`)

    for i, v in pairs(t) do
        local str = ""

        for _ = 0, tabs, 1 do
            str ..= "\t"
        end

        if i == name then
            str ..= `[{i}]: cycle`
        else
            if typeof(v) == "table" then
                printTable(v, false, i)

                continue
            else
                str ..= `[{i}]: {v}\t{typeof(v)}`
            end
        end
        

        print(str)
    end

    print(_str.."}")
end

print(_G)

printTable(_G, false, "_G", 0)