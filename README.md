# Task Manager App

A simple desktop application built using **C++** and **Qt Widgets**. The Task Manager allows users to add, edit, delete, save, load, and manage tasks effectively.

## Features

- **Add Task**: Add a new task to the list.
- **Edit Task**: Edit an existing task.
- **Delete Task**: Delete selected tasks.
- **Clear All Tasks**: Remove all tasks from the list.
- **Save Tasks**: Save tasks to a file. Options to save all tasks or only selected tasks.
- **Load Tasks**: Load tasks from a saved file.

---

## Tools and Frameworks Used

- **Qt Framework**:
  - **Type**: Qt Widgets Application.
  - **Build System**: qmake.
  - **IDE**: Qt Creator.
- **C++**: The application logic is written in modern C++.
- **QListWidget**: For task display and management.
- **QPushButton**: For actions like Add, Edit, Delete, etc.
- **QMessageBox**: For alerts, confirmations, and error messages.
- **QFileDialog**: For file selection dialogs.
- **QInputDialog**: For user input during task editing.

---

## How to Run the App

1. Install the Qt Development Environment:
   - [Download Qt](https://www.qt.io/download) and install it.
   - Use the **Qt Creator IDE**.
2. Clone this repository or create a new project.
3. Select **Qt Widgets Application** when setting up the project.
4. Add the provided source files (`.cpp`, `.h`, and `.ui`) to the project.
5. Build the project:
   - Use `qmake` as the build system.
   - Compile the project using the **Run** button in Qt Creator.

---

## Functionality Walkthrough

### 1. Add Task

- **How it works**:
  - User types a task into the input field and clicks **Add**.
  - The task is added to the list.
- **Code**:
  ```cpp
  void MainWindow::handleAddTask() {
      QString task = ui->taskInput->text(); // Get task from input
      if (!task.isEmpty()) {
          ui->taskList->addItem(task); // Add task to list
          ui->taskInput->clear(); // Clear input field
      } else {
          QMessageBox::warning(this, "Warning", "Task cannot be empty!");
      }
  }
  ```

---

### 2. Edit Task

- **How it works**:
  - User selects a task from the list and clicks **Edit**.
  - A dialog opens, allowing the user to modify the task text.
- **Code**:
  ```cpp
  void MainWindow::handleEditTask() {
      QListWidgetItem* selectedItem = ui->taskList->currentItem(); // Get selected task
      if (!selectedItem) {
          QMessageBox::information(this, "Edit Task", "Please select a task to edit.");
          return;
      }
      QString newText = QInputDialog::getText(this, "Edit Task", "Edit the task:", QLineEdit::Normal, selectedItem->text());
      if (!newText.isEmpty()) {
          selectedItem->setText(newText); // Update task text
      } else {
          QMessageBox::warning(this, "Edit Task", "Task text cannot be empty.");
      }
  }
  ```

---

### 3. Delete Task

- **How it works**:
  - User selects one or more tasks and clicks **Delete**.
  - The selected tasks are removed after confirmation.
- **Code**:
  ```cpp
  void MainWindow::handleDeleteTask() {
      QList<QListWidgetItem*> selectedItems = ui->taskList->selectedItems();
      if (selectedItems.isEmpty()) {
          QMessageBox::information(this, "Delete Task", "Please select a task to delete.");
          return;
      }
      QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Task", "Are you sure you want to delete the selected task(s)?", QMessageBox::Yes | QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          for (QListWidgetItem* item : selectedItems) {
              delete item;
          }
      }
  }
  ```

---

### 4. Clear All Tasks

- **How it works**:
  - User clicks **Clear All** to remove all tasks after confirmation.
- **Code**:
  ```cpp
  void MainWindow::handleClearAllTasks() {
      if (ui->taskList->count() == 0) {
          QMessageBox::information(this, "Clear All", "There are no tasks to clear.");
          return;
      }
      QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear All Tasks", "Are you sure you want to clear all tasks?", QMessageBox::Yes | QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          ui->taskList->clear();
      }
  }
  ```

---

### 5. Save Tasks

- **How it works**:
  - User clicks **Save** and chooses to save all tasks or selected tasks.
  - Tasks are saved to a file (`tasks.txt` or a user-specified file name).
- **Code**:
  ```cpp
  void MainWindow::handleSaveTasks() {
      // Save logic depending on user choice (all or selected tasks)
      if (ui->taskList->selectionMode() == QAbstractItemView::MultiSelection) {
          // Save selected tasks
      } else {
          // Save all tasks
      }
  }
  ```

---

### 6. Load Tasks

- **How it works**:
  - User clicks **Load** and selects a file containing tasks.
  - Tasks are loaded into the list, replacing existing ones.
- **Code**:
  ```cpp
  void MainWindow::handleLoadTasks() {
      QString fileName = QFileDialog::getOpenFileName(this, "Open Task File", "", "Text Files (*.txt);;All Files (*)");
      if (fileName.isEmpty()) return;
      QFile file(fileName);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QTextStream in(&file);
          ui->taskList->clear();
          while (!in.atEnd()) {
              ui->taskList->addItem(in.readLine());
          }
          file.close();
      }
  }
  ```

---

## Project Structure

- **Headers (`.h`)**:
  - Contains class declarations (e.g., `MainWindow`).
- **Sources (`.cpp`)**:
  - Contains class definitions and logic for each feature.
- **Forms (`.ui`)**:
  - Contains the user interface designed in Qt Designer.
- **Project File (`.pro`)**:

  - Configures the build system (qmake):

    QT += core gui widgets
    SOURCES += main.cpp mainwindow.cpp
    HEADERS += mainwindow.h
    FORMS += mainwindow.ui
