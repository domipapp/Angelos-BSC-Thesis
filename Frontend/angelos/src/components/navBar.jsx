import React from "react";
import { Link, NavLink } from "react-router-dom";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faMagnifyingGlassChart } from "@fortawesome/free-solid-svg-icons";
const Navbar = ({ ids }) => {
  return (
    <nav
      className="navbar navbar-expand  navbar-light"
      style={{ backgroundColor: "#a7826ca2" }}>
      <div className="container-fluid">
        <Link className="navbar-brand" to="/">
          <FontAwesomeIcon icon={faMagnifyingGlassChart} flip="horizontal" />
          Angelos
        </Link>
        <div className="collapse navbar-collapse">
          <ul className="navbar-nav">
            <li className="nav-item dropdown">
              <a
                className="nav-link dropdown-toggle"
                href="#"
                id="navbarDropdown"
                role="button"
                data-bs-toggle="dropdown"
                aria-expanded="false">
                Show sensor
              </a>
              <ul className="dropdown-menu" aria-labelledby="navbarDropdown">
                {ids.map((id) => (
                  <NavLink
                    className="dropdown-item"
                    to={`/sensor/${id}`}
                    key={id}>
                    {id}
                  </NavLink>
                ))}
              </ul>
            </li>
          </ul>
        </div>
      </div>
    </nav>
  );
};

export default Navbar;
