program fibonacci(output);
function fib(a: integer): integer;
begin
	if a < 2 then return 1
	else return fib(a-1)+fib(a-2)
end;
begin
    print fib(6)
end.