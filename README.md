# LinkUp — Shared Scheduling App

LinkUp is a shared scheduling application developed for my Software Engineering II class. Built with C++ and Qt Creator, the application uses an SQLite database to manage user accounts, passwords, and scheduled activities.

The goal of LinkUp is to make it easier for friends and coworkers to coordinate schedules, share activities, and compare their availability using share codes.

## Features

- **User Accounts:** Create and manage individual user accounts.
- **Login System:** Verify user credentials using information stored in the database.
- **Activity Scheduling:** Create and manage scheduled activities.
- **Schedule Sharing:** Use share codes to connect with others and compare schedules.
- **Database Storage:** Save account information and scheduled activities using SQLite.
- **Desktop Interface:** Interact with the application through a Qt-based graphical user interface.

## Technologies

- **C++** — Application logic.
- **Qt Creator / Qt** — Desktop interface and development.
- **SQLite** — Database storage and management.
- **SQL** — Database queries and operations.

## Database Implementation

SQLite is a core part of LinkUp. It stores user information and scheduled activities, allowing the application to retrieve data when needed.

The database supports three main areas:

### User Accounts
Stores account information and associates each user with their own scheduled activities.

### Passwords and Login
The application currently stores passwords in plaintext in the SQLite database and uses them for basic login verification. Secure password hashing is planned for a future update.

**Note:** LinkUp is a class project and should not be used with real user credentials in its current state.

### Scheduled Activities and Share Codes
Stores scheduled activities associated with user accounts. Share codes allow users to connect with others and compare their schedules, supporting the application's goal of making group planning easier.

## Getting Started

### Requirements
- Qt Creator
- A compatible C++ compiler
- Qt with the required SQLite database support

### Setup
1. Clone this repository:

   ```bash
   git clone <repository-url>
   ```

2. Open the project in Qt Creator.
3. Configure the appropriate Qt kit and compiler.
4. Build and run the application.

The exact setup may vary depending on the project's Qt version and configuration.

## Future Improvements

- Implement secure password hashing.
- Improve account security and input validation.
- Expand schedule comparison features.
- Improve share-code functionality.
- Add more database validation and error handling.

## About the Project

LinkUp was created for a Software Engineering II course to practice desktop application development, database implementation, and software design. The project combines user account management and scheduling features into one application focused on helping people coordinate their plans.

---

**LinkUp — Connect, schedule, and plan together.**
