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

print(test.somemethod());
print(getmetatable(test))
print(test)
print(table)

print("나다 이 잡것들아. ")
print(test.toMoneyFace("나다 이 잡것들아. "))
-- print(test.toMoneyFace())
print(test.toMoneyFace(nil))
print(test.toMoneyFace(12134))