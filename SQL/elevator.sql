-- MySQL dump 10.13  Distrib 8.4.9, for Linux (x86_64)
--
-- Host: localhost    Database: elevator
-- ------------------------------------------------------
-- Server version	8.4.9-0ubuntu0.26.04.1

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
INSERT INTO `CANnodes` VALUES (100,1,'SC',0,0),(101,2,'EC',0,0),(200,3,'CC',0,0),(201,4,'F1',0,0),(202,5,'F2',0,0),(203,6,'F3',0,0);
/*!40000 ALTER TABLE `CANnodes` ENABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `elevatorNetwork`
--

LOCK TABLES `elevatorNetwork` WRITE;
/*!40000 ALTER TABLE `elevatorNetwork` DISABLE KEYS */;
INSERT INTO `elevatorNetwork` VALUES (1,'SC','RPi','CAN/LAN','Online'),(2,'EC','Arduino','CAN','Online'),(3,'CC','STM32','CAN','Online'),(4,'F1','STM32','CAN','Online'),(5,'F2','STM32','CAN','Online'),(6,'F3','STM32','CAN','Online'),(7,'Website','Web Server','LAN','Online'),(10,'AS','As','AS','ASD');
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

-- Dump completed on 2026-07-11 12:28:51
