program example(input,output);
var x, y : integer;
function gcd(a, b: integer): integer;
begin
	if b = 0 then return a
	else return gcd(b, a mod b)
end;
begin
	x := 125;
	y := 75;
	writeln gcd(x, y)
end.