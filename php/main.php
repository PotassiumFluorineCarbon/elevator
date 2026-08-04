<?php

require_once 'Direction.php';
require_once 'Sensor.php';
require_once 'DoorSensor.php';
require_once 'WeightSensor.php';
require_once 'ArrivalSensor.php';
require_once 'ElevatorCar.php';
require_once 'FloorNode.php';
require_once 'ElevatorController.php';

echo "=== SENSORS & DEMOING INTERFACE ===\n";
$doorSensor = new DoorSensor();
$weightSensor = new WeightSensor();
$arrivalSensor = new ArrivalSensor();

$sensors = [$doorSensor, $weightSensor, $arrivalSensor];
foreach ($sensors as $sensor) {
    $sensor->testSensor(); // Interface polymorphism
}

echo "\n=== STATIC PROPERTIES & ENCAPSULATION ===\n";
echo "Max Weight Limit (Static Method): " . WeightSensor::getMaxWeightLimit() . " kg\n";

$weightSensor->setCurrentWeight(1050.0);
echo "Current Weight: " . $weightSensor->getCurrentWeight() . " kg\n";
echo "Is Overloaded? " . ($weightSensor->isOverloaded() ? "YES" : "NO") . "\n";

echo "\n=== CARS & CONTROLLER ===\n";
$car1 = new ElevatorCar(1);
$car2 = new ElevatorCar(2);
$floor1 = new FloorNode(1);
$floor2 = new FloorNode(2);

$controller = new ElevatorController();
$controller->addElevatorCar($car1);
$controller->addElevatorCar($car2);
$controller->addFloorNode($floor1);
$controller->addFloorNode($floor2);

echo "Total Cars Created (Static Property): " . ElevatorCar::getTotalCarsCreated() . "\n";

echo "\n=== SYSTEM INTERACTIONS ===\n";
// Floor request
$floor1->pressUpButton();
$controller->receiveFloorRequest(1, Direction::UP);
$controller->dispatchElevator();

// Car movement
$car1->moveUp();
$controller->updateCarPosition($car1->getId(), $car1->getCurrentFloor());
$car1->stop();
$car1->openDoors();
$car1->closeDoors();

// Floor request reset
$floor1->clearUpRequest();

echo "\n=== ALL OBJECT INSTANTIATIONS SUCCESSFUL ===\n";
