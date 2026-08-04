<?php

require_once 'Sensor.php';

class ArrivalSensor implements Sensor 
{
    private int $detectedFloor = 1;
    private bool $status = true;

    public function detectFloor(): int 
    {
        return $this->detectedFloor;
    }

    public function setDetectedFloor(int $floor): void 
    {
        $this->detectedFloor = $floor;
    }

    public function getStatus(): bool 
    {
        return $this->status;
    }

    public function testSensor(): void 
    {
        echo "[ArrivalSensor] Diagnostic test: OK.\n";
    }
}
