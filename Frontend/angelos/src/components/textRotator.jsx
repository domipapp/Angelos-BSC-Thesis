import React from "react";
import "../css/TextRotator.css"; // Create this CSS file

const TextRotator = ({ username }) => {
  return (
    <div className="text-rotator">
      <div className="text-content">
        <h1 className="username">Hello, {username}!</h1>
        <h2 className="welcomeMessage">Welcome to Angelos!</h2>
      </div>
    </div>
  );
};

export default TextRotator;
