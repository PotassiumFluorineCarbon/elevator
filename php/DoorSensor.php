<?php

require_once 'Sensor.php';

class DoorSensor implements Sensor 
{
    private bool $isObstructed = false;
    private bool $status = true;

    // Encapsulation: Getter and Setter
    public function isObstructed(): bool 
    {
        return $this->isObstructed;
    }

    public function setObstructed(bool $obstructed): void 
    {
        $this->isObstructed = $obstructed;
    }

    public function getStatus(): bool 
    {
        return $this->status;
    }

    public function testSensor(): void 
    {
        echo "[DoorSensor] Diagnostic test: OK.\n";
    }
}
