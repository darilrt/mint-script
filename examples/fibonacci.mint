fib(n: int) -> int {
    if n < 2 { return n }
    fib(n - 1) + fib(n - 2)
}

print(fib(20).ToStr())