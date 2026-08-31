program recordExample(output);
type TStudent := record
    Name  : string;
    Age   : integer;
    GPA   : real;
end;
var x : integer;
var Student : TStudent;
begin
  Student.Name := "Max G.";
  Student.Age := 39;
  Student.GPA := 4;
  writeln Student 
end .
