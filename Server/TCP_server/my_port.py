from typing import List, Tuple


class MyPort:
    # logic is: [port number, is it being used?, was it unable to bind?]
    _ports: List[Tuple[int, bool, bool]] = []

    def __init__(self, portStart, portMax, portEnd):
        self.MAX_PORT_NUMBERS = portMax  # Maximum number of valid ports to be used
        self.PORT_START = portStart  # Starting point for assignable ports
        self.PORT_END = portEnd  # Ending point for assignable ports
        if portEnd - portStart > portMax:
            self._set_ports()  # Initiate ports
        else:
            raise ValueError

    # Set ports to not being used
    def _set_ports(self):
        for i in range(self.MAX_PORT_NUMBERS):
            self._ports.append([self.PORT_START + i, False, False])

    # Return the next port that is not used and set it to used
    def get_next_port(self):
        for i, (port_num, used, error) in enumerate(self._ports):
            if not used and not error:
                self._ports[i] = [port_num, True, False]
                return port_num
        raise OverflowError  # All ports are used

    # Return a list of all ports
    def _get_port_numbers(self):
        return [port for port, _, _ in self._ports]

    # Is port in allowed range
    def _is_range_valid_port(self, num):
        if num not in range(self.PORT_START, self.PORT_END):
            return False
        return True

    # Is port already in ports list
    def _is_port_usable(self, num):
        if num in self._ports:
            return False
        return True

    # Changes the port to a new valid one. Overwrites the ports list
    # Returns the new port
    def get_new_port(self, port):
        try:
            index = self._ports.index([port, True, False])
        except ValueError:
            raise ValueError
        newPortNum = self.PORT_START

        while True:
            if self._is_range_valid_port(newPortNum) and self._is_port_usable(
                newPortNum
            ):
                self._ports[index] = [newPortNum, True, False]
                break
            else:
                newPortNum += 1
        return self._ports[index][0]

    # Set a port status
    def set_port(self, port, used, error):
        for i, (port_num, _, _) in enumerate(self._ports):
            if port_num == port:
                self._ports[i] = [port_num, used, error]
                return True
        return False

    # Return the list of ports
    def get_port_list(self):
        return self._ports
