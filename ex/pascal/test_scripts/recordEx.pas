program recordExample(output);
type TStudent := record
    Name  : string;
    Age   : integer;
    GPA   : real;
end;
var Student : TStudent;
begin
  Student.Age := 14;
  writeln Student.Age 
end .