program example(input,output);
var x, y : integer;
function gcd(a, b: integer): integer;
begin
	if b = 0 then gcd := a
	else gcd := gcd(b, a mod b)
end;
begin
	x := 350;
	y := 65;
	print gcd(x,y)
end.