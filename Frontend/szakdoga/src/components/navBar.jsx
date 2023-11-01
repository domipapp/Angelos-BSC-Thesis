import React, { Component } from "react";
import { Link, NavLink } from "react-router-dom";

class NavBar extends Component {
  state = {};
  render() {
    const { ids } = this.props;

    return (
      <nav className="navbar navbar-expand-sm  navbar-light bg-light ">
        <Link className="navbar-brand" to="/">
          Vidly
        </Link>
        <div className="collapse navbar-collapse" id="navbarNavAltMarkup">
          <div className="navbar-nav">
            {ids.map((id) => (
              <NavLink
                className="nav-item nav-link"
                to={`/sensor/${id}`}
                key={id}>
                {id}
              </NavLink>
            ))}
          </div>
        </div>
      </nav>
    );
  }
}

export default NavBar;
