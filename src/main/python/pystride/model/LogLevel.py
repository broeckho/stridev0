from enum import Enum

class LogLevel(Enum):
    NONE = 0
    TRANSMISSIONS = 1
    CONTACTS = 2

    def __str__(self):
        return self.name.title()
