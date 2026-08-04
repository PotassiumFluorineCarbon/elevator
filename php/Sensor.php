<?php

interface Sensor 
{
    public function getStatus(): bool;
    public function testSensor(): void;
}
