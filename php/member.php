<?php
    session_start();
        if(isset($_SESSION['username'])){
            echo "Welcome," . $_SESSION['username'] . "!";

            //Add 'members only' content here
            echo "<p>Members only content - for your eyes only</p>";

            echo "Click to <a href='logout.php'>Logout</a>";
        } else {
            echo "You must be logged in!";
        }
?>