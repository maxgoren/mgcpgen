program arrayEx(output);
var a : array [ 1 .. 5 ] of integer; 
var x : integer; 
begin
    a[1] := 15;
    a[2] := 86;
    a[3] := 24;
    a[4] := 101;
    a[5] := 11;
    x := 1;
    while (5 < 5) do
    begin 
        writeln a[x];
        x := x + 1 
    end 
end.