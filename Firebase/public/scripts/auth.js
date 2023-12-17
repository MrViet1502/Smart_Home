// listen for auth status changes
const loginElement = document.querySelector('#login-form');

auth.onAuthStateChanged(user => {
    if (user) {
        console.log("user logged in");
        console.log(user);
        setupUI(user);
        var uid = user.uid;
        console.log(uid);

        // Redirect to index.html after successful login
        window.location.href = 'dashboard1.html';
    } else {
        console.log("user logged out");
        setupUI();
    }
});

const setupUI = (user) => {
    if (user) {
        //toggle UI elements

        // get user UID to get data from database
        var uid = user.uid;
        console.log(uid);

        // Database paths (with user UID)

        // if user is logged out
    } else {
        // toggle UI elements
    }
}

// login
const loginForm = document.querySelector('#login-form');
loginForm.addEventListener('submit', (e) => {
    e.preventDefault();
    // get user info
    const email = loginForm['input-email'].value;
    const password = loginForm['input-password'].value;
    // log the user in
    auth.signInWithEmailAndPassword(email, password).then((cred) => {
        // close the login modal & reset form
        loginForm.reset();
        console.log(email);
    })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;
            document.getElementById("error-message").innerHTML = errorMessage;
            console.log(errorMessage);
        });
});

// logout
const logout = document.querySelector('#logout-link');
logout.addEventListener('click', (e) => {
    e.preventDefault();
    auth.signOut();
});
