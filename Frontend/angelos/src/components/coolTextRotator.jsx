import React from "react";
import "../css/CoolTextRotator.css"; // Create this CSS file

const CoolTextRotator = ({ username }) => {
  return (
    <div className="cool-text-rotator">
      <div className="cool-text-content">
        <h1 className="username">Hello, {username}!</h1>
        <h2 className="welcomeMessage">Welcome to Angelos!</h2>
      </div>
    </div>
  );
};

export default CoolTextRotator;
