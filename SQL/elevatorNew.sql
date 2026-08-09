-- MySQL dump 10.13  Distrib 8.4.10, for Linux (x86_64)
--
-- Host: localhost    Database: elevator
-- ------------------------------------------------------
-- Server version	8.4.10-0ubuntu0.26.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `CAN_messages`
--

DROP TABLE IF EXISTS `CAN_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `CAN_messages` (
  `MessageID` int NOT NULL AUTO_INCREMENT,
  `CANID` int NOT NULL,
  `MessageData` int NOT NULL,
  `Timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`MessageID`),
  KEY `CANID` (`CANID`),
  CONSTRAINT `CAN_messages_ibfk_1` FOREIGN KEY (`CANID`) REFERENCES `CANnodes` (`CANID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CAN_messages`
--

LOCK TABLES `CAN_messages` WRITE;
/*!40000 ALTER TABLE `CAN_messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `CAN_messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `CANnodes`
--

DROP TABLE IF EXISTS `CANnodes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `CANnodes` (
  `CANID` int NOT NULL,
  `NodeID` int NOT NULL,
  `NodeName` varchar(100) NOT NULL,
  `MessagesSent` int DEFAULT '0',
  `MessagesReceived` int DEFAULT '0',
  PRIMARY KEY (`CANID`),
  KEY `fk_node` (`NodeID`),
  CONSTRAINT `fk_node` FOREIGN KEY (`NodeID`) REFERENCES `elevatorNetwork` (`nodeID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CANnodes`
--

LOCK TABLES `CANnodes` WRITE;
/*!40000 ALTER TABLE `CANnodes` DISABLE KEYS */;
INSERT INTO `CANnodes` VALUES (100,1,'SC',0,0),(101,2,'EC',0,0),(200,3,'CC',0,0),(201,4,'F1',0,0),(202,5,'F2',0,0),(203,6,'F3',0,0),(300,7,'Website',0,0);
/*!40000 ALTER TABLE `CANnodes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Diagnostics`
--

DROP TABLE IF EXISTS `Diagnostics`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Diagnostics` (
  `DiagnosticID` int NOT NULL AUTO_INCREMENT,
  `NodeID` int NOT NULL,
  `Timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `Message` text NOT NULL,
  PRIMARY KEY (`DiagnosticID`),
  KEY `NodeID` (`NodeID`),
  CONSTRAINT `Diagnostics_ibfk_1` FOREIGN KEY (`NodeID`) REFERENCES `elevatorNetwork` (`nodeID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Diagnostics`
--

LOCK TABLES `Diagnostics` WRITE;
/*!40000 ALTER TABLE `Diagnostics` DISABLE KEYS */;
INSERT INTO `Diagnostics` VALUES (7,1,'2026-07-20 14:54:45','Emergency stop activated');
/*!40000 ALTER TABLE `Diagnostics` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ElevatorCommands`
--

DROP TABLE IF EXISTS `ElevatorCommands`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ElevatorCommands` (
  `CommandID` int NOT NULL AUTO_INCREMENT,
  `CANID` int NOT NULL,
  `Data` int NOT NULL,
  `Timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `Status` enum('pending','complete') DEFAULT 'complete',
  PRIMARY KEY (`CommandID`),
  UNIQUE KEY `CANID` (`CANID`,`Data`),
  CONSTRAINT `ElevatorCommands_ibfk_1` FOREIGN KEY (`CANID`) REFERENCES `CANnodes` (`CANID`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ElevatorCommands`
--

LOCK TABLES `ElevatorCommands` WRITE;
/*!40000 ALTER TABLE `ElevatorCommands` DISABLE KEYS */;
INSERT INTO `ElevatorCommands` VALUES (1,100,5,'2026-07-15 15:20:19','complete'),(2,100,6,'2026-07-15 15:20:19','complete'),(3,100,7,'2026-07-15 15:20:19','complete'),(4,200,1,'2026-08-04 17:03:49','complete'),(5,200,2,'2026-08-04 17:03:50','complete'),(6,200,3,'2026-08-04 17:03:50','complete'),(7,201,4,'2026-08-04 17:03:51','complete'),(8,202,2,'2026-08-04 17:03:52','complete'),(9,202,4,'2026-08-04 17:03:51','complete'),(10,203,2,'2026-08-04 17:03:52','complete'),(13,300,1,'2026-08-04 17:03:53','complete'),(14,300,2,'2026-08-04 17:03:54','complete');
/*!40000 ALTER TABLE `ElevatorCommands` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ElevatorStatus`
--

DROP TABLE IF EXISTS `ElevatorStatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ElevatorStatus` (
  `CurrentFloor` int NOT NULL,
  `Direction` enum('up','down','idle') NOT NULL,
  `Timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ElevatorStatus`
--

LOCK TABLES `ElevatorStatus` WRITE;
/*!40000 ALTER TABLE `ElevatorStatus` DISABLE KEYS */;
INSERT INTO `ElevatorStatus` VALUES (1,'up','2026-07-24 18:48:19');
/*!40000 ALTER TABLE `ElevatorStatus` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Users`
--

DROP TABLE IF EXISTS `Users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Users` (
  `UserID` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(50) NOT NULL,
  `Password` varchar(255) NOT NULL,
  PRIMARY KEY (`UserID`),
  UNIQUE KEY `Username` (`Username`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Users`
--

LOCK TABLES `Users` WRITE;
/*!40000 ALTER TABLE `Users` DISABLE KEYS */;
INSERT INTO `Users` VALUES (1,'IAmAdmin','$2y$12$HQgaTaujASBVkXfPMC701Oh.wfToXBD1aHTIqamGk46omYLCxC/ia');
/*!40000 ALTER TABLE `Users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `elevatorNetwork`
--

DROP TABLE IF EXISTS `elevatorNetwork`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `elevatorNetwork` (
  `nodeID` int NOT NULL AUTO_INCREMENT,
  `NodeName` varchar(100) NOT NULL,
  `nodeType` varchar(50) NOT NULL,
  `NetworkType` varchar(50) NOT NULL,
  `Status` varchar(20) NOT NULL,
  PRIMARY KEY (`nodeID`),
  UNIQUE KEY `NodeName` (`NodeName`),
  KEY `idx_networkType` (`NetworkType`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `elevatorNetwork`
--

LOCK TABLES `elevatorNetwork` WRITE;
/*!40000 ALTER TABLE `elevatorNetwork` DISABLE KEYS */;
INSERT INTO `elevatorNetwork` VALUES (1,'SC','RPi','CAN/LAN','10'),(2,'EC','Arduino','CAN','Online'),(3,'CC','STM32','CAN','10'),(4,'F1','STM32','CAN','Online'),(5,'F2','STM32','CAN','Online'),(6,'F3','STM32','CAN','Online'),(7,'Website','HTML','LAN','Online');
/*!40000 ALTER TABLE `elevatorNetwork` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-08-09 13:01:26
