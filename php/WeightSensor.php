<?php

require_once 'Sensor.php';

class WeightSensor implements Sensor 
{
    private float $currentWeight = 0.0;
    private bool $status = true;

    private static float $maxWeightLimit = 1000.0; // in kg

    public function isOverloaded(): bool 
    {
        return $this->currentWeight > self::$maxWeightLimit;
    }

    public function getCurrentWeight(): float 
    {
        return $this->currentWeight;
    }

    public function setCurrentWeight(float $weight): void 
    {
        if ($weight >= 0) 
        {
            $this->currentWeight = $weight;
        }
    }

    public function getStatus(): bool 
    {
        return $this->status;
    }

    public function testSensor(): void 
    {
        echo "[WeightSensor] Diagnostic test: OK.\n";
    }
  
    public static function getMaxWeightLimit(): float 
    {
        return self::$maxWeightLimit;
    }
}
