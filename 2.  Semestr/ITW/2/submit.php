<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve form data
    $name = $_POST["name"];
    $email = $_POST["email"];
    $reason = $_POST["reason_for_contact"];
    $message = $_POST["description"];

    // Display the submitted data
    echo "Name: " . $name . "<br>";
    echo "Email: " . $email . "<br>";
    echo "Message: " . $reason . "<br>";
    echo "Message: " . $message . "<br>";
}
?>