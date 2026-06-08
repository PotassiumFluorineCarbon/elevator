document.addEventListener('DOMContentLoaded', () => {

    const elUsername = document.querySelector('input[name="username"]');
    const elPassword = document.querySelector('input[name="password"]');
    const elMsg      = document.getElementById('feedback');

    if (!elUsername || !elPassword || !elMsg) {
        console.error("Form elements not found!");
        return;
    }

    window.addEventListener('load', () => {elUsername.focus()})

    function checkUsername() {
        const min = 7;
        if (elUsername.value.trim().length < min) {
            elMsg.innerHTML = `<span class="text-danger">Username must be at least ${min} characters long.</span>`;
        } else {
            elMsg.innerHTML = '';
        }
    }

    function checkPassword() {
        const min = 7;
        if (elPassword.value.trim().length < min) {
            elMsg.innerHTML = `<span class="text-danger">Password must be at least ${min} characters long.</span>`;
        } else {
            elMsg.innerHTML = '';
        }
    }

    // Live validation while typing + on blur
    elUsername.addEventListener('input', checkUsername);
    elUsername.addEventListener('blur', checkUsername);
    
    elPassword.addEventListener('input', checkPassword);
    elPassword.addEventListener('blur', checkPassword);

    console.log("✅ Real-time validation loaded");
});