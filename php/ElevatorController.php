<?php

require_once 'Direction.php';
require_once 'ElevatorCar.php';
require_once 'FloorNode.php';

class ElevatorController 
{
    private array $cars = [];

    private array $floors = [];

    public function addElevatorCar(ElevatorCar $car): void 
    {
        $this->cars[$car->getId()] = $car;
    }

    public function addFloorNode(FloorNode $floor): void 
    {
        $this->floors[$floor->getFloorNumber()] = $floor;
    }

    public function receiveFloorRequest(int $floor, Direction $direction): void 
    {
        echo "[Controller] Received request from Floor {$floor} to go {$direction->name}.\n";
    }

    public function receiveCarRequest(int $elevatorId, int $destinationFloor): void 
    {
        echo "[Controller] Car {$elevatorId} received destination Floor {$destinationFloor}.\n";
    }

    public function dispatchElevator(): void 
    {
        echo "[Controller] Dispatching optimal elevator car to serve requests.\n";
    }

    public function updateCarPosition(int $elevatorId, int $floor): void 
    {
        echo "[Controller] System updated: Elevator {$elevatorId} is now at Floor {$floor}.\n";
    }
}
