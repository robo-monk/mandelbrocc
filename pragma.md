- everything is a function
- code can be passed as parameter

## Hello world
puts("hello world")

## Conditional
if(lt(5, 8), {
    puts("hello")
})

## Define variables and procedures
<!-- define("if", {

} -->
define if {
    a
}

define if(bool a, proc b) {
    ...
}

define puts(s: string): void {
}


define hello
    puts("bing bong")
end

define add($a: int, $b: int, $ret: int):
    $ret <- $a + $b
end



## Macro 

macro push-op
    flfdadf
end
