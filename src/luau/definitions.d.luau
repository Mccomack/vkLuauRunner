export type stringable = string | number | {[any]: any} | boolean | nil

declare engine: {
    smth: () -> nil,
}

declare class Enum
    member: string
    function something(self): number
end

declare Enum: Enum

declare function add(v1: number, v2: number): number
declare function security(): string
declare function printSecurityLevel(): nil

-- declare class test
--     function somemethod(): string
--     function toMoneyFace(str: string): string
-- end

-- declare test: test
declare test: {
    somemethod: () -> string,
    toMoneyFace: (str: stringable) -> string;
}