<?php

class FloorNode 
{
    private int $floorNumber;
    private bool $upButtonPressed = false;
    private bool $downButtonPressed = false;

    public function __construct(int $floorNumber) 
    {
        $this->floorNumber = $floorNumber;
    }

    public function getFloorNumber(): int 
    {
        return $this->floorNumber;
    }

    public function pressUpButton(): void 
    {
        $this->upButtonPressed = true;
        echo "Floor {$this->floorNumber}: UP button pressed.\n";
    }

    public function pressDownButton(): void 
    {
        $this->downButtonPressed = true;
        echo "Floor {$this->floorNumber}: DOWN button pressed.\n";
    }

    public function clearUpRequest(): void 
    {
        $this->upButtonPressed = false;
        echo "Floor {$this->floorNumber}: UP request cleared.\n";
    }

    public function clearDownRequest(): void 
    {
        $this->downButtonPressed = false;
        echo "Floor {$this->floorNumber}: DOWN request cleared.\n";
    }
}
