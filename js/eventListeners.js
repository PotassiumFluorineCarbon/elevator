document.addEventListener('DOMContentLoaded', () => {

    // ==========================================
    // 1. Login Form Validation
    // ==========================================
    const loginForm = document.getElementById('login');
    
    if (loginForm) {
        const elUsername = document.querySelector('input[name="username"]');
        const elPassword = document.querySelector('input[name="password"]');
        const elMsg      = document.getElementById('feedback');

        if (!elUsername || !elPassword || !elMsg) {
            console.error("Login form elements not found!");
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
            
            // Stop form submission if invalid
            if (!isUserValid || !isPassValid) {
                event.preventDefault();
            }
        });

        console.log("Real-time login validation loaded");
    }

    // ==========================================
    // 2. Request Access Form Validation
    // ==========================================
    const accessForm = document.getElementById('requestAccess');
    
    if (accessForm) {
        // Account Fields
        const elUsername  = document.querySelector('input[name="username"]');
        const elPassword  = document.querySelector('input[name="password"]');
        
        // Contact Fields
        const elFirstName = document.querySelector('input[name="firstname"]');
        const elLastName  = document.querySelector('input[name="lastname"]');
        const elEmail     = document.querySelector('input[name="email"]');
        const elMsg       = document.getElementById('feedback');

        if (elMsg) {
            function validateAccessForm() {
                elMsg.innerHTML = '';
                const minLen = 7;

                // Validate Username
                if (!elUsername || elUsername.value.trim().length < minLen) {
                    elMsg.innerHTML = `<span class="text-danger">Username must be at least ${minLen} characters long.</span>`;
                    return false;
                }

                // Validate Password
                const pass = elPassword.value.trim();
                if (pass.length < minLen) {
                    elMsg.innerHTML = `<span class="text-danger">Password must be at least ${minLen} characters long.</span>`;
                    return false;
                }
                if (!(/[A-Z]/.test(pass)) || !(/[0-9]/.test(pass))) {
                    elMsg.innerHTML = '<span class="text-danger">Password must contain at least one uppercase letter and one number.</span>';
                    return false;
                }

                // Validate First Name
                if (!elFirstName || elFirstName.value.trim() === '') {
                    elMsg.innerHTML = '<span class="text-danger">First name is required.</span>';
                    return false;
                }

                // Validate Last Name
                if (!elLastName || elLastName.value.trim() === '') {
                    elMsg.innerHTML = '<span class="text-danger">Last name is required.</span>';
                    return false;
                }

                // Validate Email
                const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
                if (!elEmail || !emailRegex.test(elEmail.value.trim())) {
                    elMsg.innerHTML = '<span class="text-danger">Please enter a valid email address.</span>';
                    return false;
                }

                // Validate Status Radio
                const checkedRadio = document.querySelector('input[name="fac_or_student"]:checked');
                if (!checkedRadio) {
                    elMsg.innerHTML = '<span class="text-danger">Please select your status (Faculty or Student).</span>';
                    return false;
                }

                // Validate Car Dropdown
                const elDrivesCar = document.querySelector('select[name="drives_car"]');
                if (!elDrivesCar || elDrivesCar.value === 'no_answer') {
                    elMsg.innerHTML = '<span class="text-danger">Please answer if you drive a car.</span>';
                    return false;
                }

                return true; 
            }

            // Listen for 'blur' (clicking away) on text inputs
            if (elUsername)  elUsername.addEventListener('blur', validateAccessForm);
            if (elPassword)  elPassword.addEventListener('blur', validateAccessForm);
            if (elFirstName) elFirstName.addEventListener('blur', validateAccessForm);
            if (elLastName)  elLastName.addEventListener('blur', validateAccessForm);
            if (elEmail)     elEmail.addEventListener('blur', validateAccessForm);

            // Listen for 'change' (selecting an option) on radio buttons
            const statusRadios = document.querySelectorAll('input[name="fac_or_student"]');
            statusRadios.forEach(radio => {
                radio.addEventListener('change', validateAccessForm);
            });

            // Listen for 'change' on the car dropdown
            const elDrivesCar = document.querySelector('select[name="drives_car"]');
            if (elDrivesCar) {
                elDrivesCar.addEventListener('change', validateAccessForm);
            }

            // Listen for form submission
            accessForm.addEventListener('submit', (event) => {
                if (!validateAccessForm()) {
                    event.preventDefault(); 
                }
            });

            console.log("Request Access validation rules loaded");
        }
    }
});