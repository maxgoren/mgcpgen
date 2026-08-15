program example(output);
var y : integer;
function fib(a: integer): integer; 
begin 
	if a < 2 then return 1 
	else return fib(a-1)+fib(a-2) 
end; 
begin
	y := 6;
	writeln fib(y)
end.