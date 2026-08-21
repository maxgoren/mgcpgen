program arrayEx(output);
var a : array [ 1 .. 5 ] of integer; 
var x : integer;
    function printArr(z : array [ 1 .. 5 ] of integer) : integer;
    begin
        x := 1;
        while (x < 6) do
        begin 
            writeln z[x];
            x := x + 1 
        end 
    end;
begin
    a[1] := 15;
    a[2] := 86;
    a[3] := 24;
    a[4] := 101;
    a[5] := 11;
    printArr(a)
end.