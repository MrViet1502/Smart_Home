// your_script.js

// ... (existing code)

// logout
const logoutBtn = document.querySelector('#logout-btn');

logoutBtn.addEventListener('click', (e) => {
    e.preventDefault();

    // Đăng xuất người dùng
    auth.signOut().then(() => {
        console.log("user logged out");
        // Chuyển hướng người dùng đến trang đăng nhập sau khi đăng xuất
        window.location.href = 'index.html';
    }).catch((error) => {
        console.error("Logout error:", error);
    });
});

// ... (existing code)
