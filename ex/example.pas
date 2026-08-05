program example(input,output);
var x, y, z : integer;
function gcd(a, b: integer): integer;
begin
	if b = 0 then return a
	else return gcd(b, a mod b)
end;
function fib(a: integer): integer;
begin
	if a < 2 then return 1
	else return fib(a-1)+fib(a-2)
end;
begin
	x := 125;
	y := 75;
	z := 6;
	print gcd(x, y);
	print fib(z)
end.