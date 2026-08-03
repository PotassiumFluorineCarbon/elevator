<?php

require_once 'DoorSensor.php';
require_once 'WeightSensor.php';
require_once 'ArrivalSensor.php';

class ElevatorCar 
{
    private int $id;
    private int $currentFloor = 1;
    private bool $doorsOpen = false;

    // Composition: ElevatorCar owns its sensors
    private DoorSensor $doorSensor;
    private WeightSensor $weightSensor;
    private array $arrivalSensors = [];

    // Static property tracking how many cars were instantiated
    private static int $totalCarsCreated = 0;

    public function __construct(int $id) 
    {
        $this->id = $id;
        $this->doorSensor = new DoorSensor();
        $this->weightSensor = new WeightSensor();
        $this->arrivalSensors[] = new ArrivalSensor();

        self::$totalCarsCreated++;
    }

    // Static method to get car count
    public static function getTotalCarsCreated(): int
    {
        return self::$totalCarsCreated;
    }

    public function getId(): int 
    {
        return $this->id;
    }

    public function moveUp(): void 
    {
        $this->currentFloor++;
        echo "Car {$this->id}: Moving UP to floor {$this->currentFloor}.\n";
    }

    public function moveDown(): void 
    {
        if ($this->currentFloor > 1) 
        {
            $this->currentFloor--;
        }
        echo "Car {$this->id}: Moving DOWN to floor {$this->currentFloor}.\n";
    }

    public function stop(): void 
    {
        echo "Car {$this->id}: STOPPED at floor {$this->currentFloor}.\n";
    }

    public function openDoors(): void 
    {
        if ($this->doorSensor->isObstructed()) 
        {
            echo "Car {$this->id}: Cannot open doors — obstacle detected!\n";
            return;
        }
        $this->doorsOpen = true;
        echo "Car {$this->id}: Doors OPENED.\n";
    }

    public function closeDoors(): void 
    {
        $this->doorsOpen = false;
        echo "Car {$this->id}: Doors CLOSED.\n";
    }

    public function getCurrentFloor(): int 
    {
        return $this->currentFloor;
    }

    // Sensor getters for testing
    public function getDoorSensor(): DoorSensor 
    {
        return $this->doorSensor;
    }

    public function getWeightSensor(): WeightSensor 
    {
        return $this->weightSensor;
    }
}
