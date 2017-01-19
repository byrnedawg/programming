<!DOCTYPE html>
<html>
<body>

<?php

$cars = array("Volvo", "BMW", "Toyota");
$arrlength = count($cars);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $cars[$x];
    echo "<br>";
}

echo "<br>";
sort($cars);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $cars[$x];
    echo "<br>";
}
echo "<br>";
rsort($cars);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $cars[$x];
    echo "<br>";
}
echo "<br>";
$numbers = array(4, 6, 2, 22, 11);
$arrlength = count($numbers);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $numbers[$x];
    echo "<br>";
}
echo "<br>";
sort($numbers);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $numbers[$x];
    echo "<br>";
}
echo "<br>";
rsort($numbers);

for($x = 0; $x < $arrlength; $x++) 
{
    echo $numbers[$x];
    echo "<br>";
}
echo "<br>";
$age = array("Peter"=>"35", "Ben"=>"37", "Joe"=>"43");

foreach($age as $x => $x_value) {
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
}
echo "<br>";
asort($age); // Sort Array (Ascending Order), According to Value - asort()

foreach($age as $x => $x_value) {
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
}
echo "<br>";
ksort($age); // Sort Array (Ascending Order), According to Key - ksort()

foreach($age as $x => $x_value) {
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
}
echo "<br>";
arsort($age); // Sort Array (Descending Order), According to Value - arsort()

foreach($age as $x => $x_value) {
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
}
echo "<br>";
krsort($age); // Sort Array (Descending Order), According to Key - krsort()

foreach($age as $x => $x_value) {
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
}
?>

</body>
</html>