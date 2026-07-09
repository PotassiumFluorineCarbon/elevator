function sendDirection(direction)
{
    fetch("../php/add_request.php", {

        method: "POST",

        headers: {
            "Content-Type": "application/x-www-form-urlencoded"
        },

        body:
            "direction=" + direction

    })

    .then(response => response.text())

    .then(data => {

        document.getElementById("message").innerHTML = data;

    })

    .catch(error => {

        console.log(error);

    });
}



function requestFloor(floor)
{

    fetch("../php/add_request.php", {

        method: "POST",

        headers: {
            "Content-Type": "application/x-www-form-urlencoded"
        },

        body:
            "floor=" + floor

    })

    .then(response => response.text())

    .then(data => {

        document.getElementById("message").innerHTML = data;

    })

    .catch(error => {

        console.log(error);

    });

}