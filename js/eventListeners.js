document.addEventListener('DOMContentLoaded', () => {

    // 1. Login Form Validation
    const loginForm = document.getElementById('login');
    
    if (loginForm) {
        const elUsername = document.querySelector('input[name="username"]');
        const elPassword = document.querySelector('input[name="password"]');
        const elMsg      = document.getElementById('feedback');

        if (!elUsername || !elPassword || !elMsg) {
            console.error("Form elements not found!");
            return;
        }

        window.addEventListener('load', () => { elUsername.focus(); });

        function checkUsername() {
            const min = 7;
            if (elUsername.value.trim().length < min) {
                elMsg.innerHTML = `<span class="text-danger">Username must be at least ${min} characters long.</span>`;
                return false;
            } else {
                elMsg.innerHTML = '';
                return true;
            }
        }

        function checkPassword() {
            const min = 7;
            const password = elPassword.value.trim();
            if (password.length < min) {
                elMsg.innerHTML = `<span class="text-danger">Password must be at least ${min} characters long.</span>`;
                return false;
            }

            else if (!(/[A-Z]/.test(password)) || !(/[0-9]/.test(password))) {
                elMsg.innerHTML = '<span class="text-danger">Password must contain at least one uppercase letter and one number.</span>';
                return false;
            }

            else {
                elMsg.innerHTML = '';
                return true;
            }
        }

        elUsername.addEventListener('input', checkUsername);
        elUsername.addEventListener('blur', checkUsername);
        
        elPassword.addEventListener('input', checkPassword);
        elPassword.addEventListener('blur', checkPassword);

        loginForm.addEventListener('submit', (event) => {
            const isUserValid = checkUsername();
            const isPassValid = checkPassword();
            if (!isUserValid || !isPassValid) {
                event.preventDefault();
            }
        });

        console.log("Real-time validation loaded");
    }

    // 2. Request Access Form Validation
    const accessForm = document.getElementById('requestAccess');
    
    if (accessForm) {
        const elFirstName = document.querySelector('input[name="firstname"]');
        const elLastName  = document.querySelector('input[name="lastname"]');
        const elEmail     = document.querySelector('input[name="email"]');
        const elMsg       = document.getElementById('feedback');

        if (elMsg) {
            function validateAccessForm() {
                elMsg.innerHTML = '';

                if (!elFirstName || elFirstName.value.trim() === '') {
                    elMsg.innerHTML = '<span class="text-danger">First name is required.</span>';
                    return false;
                }

                if (!elLastName || elLastName.value.trim() === '') {
                    elMsg.innerHTML = '<span class="text-danger">Last name is required.</span>';
                    return false;
                }

                if (!elEmail || elEmail.value.trim() === '') {
                    elMsg.innerHTML = '<span class="text-danger">Email is required.</span>';
                    return false;
                }

                const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
                if (!emailRegex.test(elEmail.value.trim())) {
                    elMsg.innerHTML = '<span class="text-danger">Please enter a valid email address.</span>';
                    return false;
                }

                const checkedRadio = document.querySelector('input[name="fac_or_student"]:checked');
                if (!checkedRadio) {
                    elMsg.innerHTML = '<span class="text-danger">Please select your status (Faculty or Student).</span>';
                    return false;
                }

                const elDrivesCar = document.querySelector('select[name="drives_car"]');
                if (!elDrivesCar || elDrivesCar.value === 'no_answer') {
                    elMsg.innerHTML = '<span class="text-danger">Please answer if you drive a car.</span>';
                    return false;
                }

                return true; 
            }

            if (elFirstName) elFirstName.addEventListener('blur', validateAccessForm);
            if (elLastName)  elLastName.addEventListener('blur', validateAccessForm);
            if (elEmail)     elEmail.addEventListener('blur', validateAccessForm);

            accessForm.addEventListener('submit', (event) => {
                if (!validateAccessForm()) {
                    event.preventDefault(); 
                }
            });

            console.log("Request Access validation rules loaded");
        }
    }
});
