document.addEventListener('DOMContentLoaded', () => {
    //Event listeners example
var elUsername = document.getElementById('username');
var elPassword = document.getElementById('password');
var elMsg = document.getElementById('feedback');

if (!elUsername || !elPassword || !elMsg) {
        console.error("One or more form elements not found!");
        return;
    }
    
function checkUsername(minLength){
    if(elUsername.value.length < minLength){
        elMsg.innerHTML = '<p>Username must be ' + minLength+' characters or more</p>';
    } else {
        elMsg.innerHTML = ''; //Clear any error message
    }
}

function checkPassword(minLength){
    if(elPassword.value.length < minLength){
        elMsg.innerHTML = '<p>Password must be ' + minLength+' characters or more</p>';
    } else {
        elMsg.innerHTML = ''; //Clear any error message
    }
}
elUsername.addEventListener('input', checkUsername);
elUsername.addEventListener('blur', function() {checkUsername(7)}, false);
elPassword.addEventListener('blur', function() {checkPassword(7)}, false);
});