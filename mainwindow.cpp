#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::handleAddTask);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::handleEditTask);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteTask);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handleSaveTasks);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::handleLoadTasks);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::handleClearAllTasks);

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::handleAddTask() {
    QString task = ui->taskInput->text(); // Get text from QLineEdit
    if (!task.isEmpty()) {
        ui->taskList->addItem(task);      // Add task to QListWidget
        ui->taskInput->clear();          // Clear the input field
    } else {
        QMessageBox::warning(this, "Warning", "Task input cannot be empty!");
    }
}

void MainWindow::handleEditTask() {
    QListWidgetItem* selectedItem = ui->taskList->currentItem(); // Get the currently selected task

    if (!selectedItem) {
        QMessageBox::information(this, "Edit Task", "Please select a task to edit.");
        return; // Exit if no task is selected
    }

    QString newText = QInputDialog::getText(
        this,
        "Edit Task",
        "Edit the task:",
        QLineEdit::Normal,
        selectedItem->text()
        );

    if (newText.isEmpty()) {
        QMessageBox::warning(this, "Edit Task", "Task text cannot be empty.");
        return; // Exit if the new task text is empty
    }

    selectedItem->setText(newText); // Update the task text
    QMessageBox::information(this, "Edit Task", "Task updated successfully.");
}


void MainWindow::handleDeleteTask() {
    QList<QListWidgetItem*> selectedItems = ui->taskList->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::information(this, "Delete Task", "Please select a task to delete.");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Task",
        "Are you sure you want to delete the selected task(s)?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        for (QListWidgetItem* item : selectedItems) {
            delete item; // Deletes the selected items
        }
        QMessageBox::information(this, "Delete Task", "Selected task(s) have been deleted.");
    }
}


void MainWindow::handleSaveTasks() {
    // Check the number of tasks in the list
    int taskCount = ui->taskList->count();

    if (taskCount == 0) {
        QMessageBox::information(this, "Save Tasks", "There are no tasks to save.");
        return; // Exit if no tasks are in the list
    }

    if (taskCount == 1) {
        // If there's only 1 task, save it directly
        QString fileName = QFileDialog::getSaveFileName(this, "Save Task File", "", "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty()) {
            QMessageBox::information(this, "Save Tasks", "No file name specified.");
            return; // Exit if no file name is provided
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->taskList->item(0)->text() << "\n"; // Save the single task
            file.close();
            QMessageBox::information(this, "Save Tasks", "Task saved successfully to " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Could not save the task to the specified file.");
        }
        return; // Exit after saving the single task
    }

    // If there are multiple tasks, proceed with the usual logic
    if (ui->taskList->selectionMode() == QAbstractItemView::MultiSelection) {
        // User has already been prompted to select tasks; now check for selected items
        QList<QListWidgetItem*> selectedItems = ui->taskList->selectedItems();
        if (selectedItems.isEmpty()) {
            QMessageBox::warning(this, "No Task Selected", "Please select at least one task to save.");
            return; // Exit if no tasks are selected
        }

        // Ask for a file name to save
        QString fileName = QFileDialog::getSaveFileName(this, "Save Task File", "", "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty()) {
            QMessageBox::information(this, "Save Tasks", "No file name specified.");
            return; // Exit if no file name is provided
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (QListWidgetItem* item : selectedItems) {
                out << item->text() << "\n"; // Write selected tasks to the file
            }
            file.close();
            QMessageBox::information(this, "Save Tasks", "Selected tasks saved successfully to " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Could not save tasks to the specified file.");
        }

        // Reset selection mode back to single selection
        ui->taskList->setSelectionMode(QAbstractItemView::SingleSelection);
        return; // Exit after saving selected tasks
    }

    // Initial Save button click: Ask for saving option
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Save Options",
        "Do you want to save all tasks?\n\nPress 'Yes' for all tasks or 'No' to select tasks.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

    // Handle Cancel case
    if (reply == QMessageBox::Cancel) {
        return; // Exit if the user cancels
    }

    if (reply == QMessageBox::Yes) {
        // Save all tasks
        QString fileName = QFileDialog::getSaveFileName(this, "Save Task File", "", "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty()) {
            QMessageBox::information(this, "Save Tasks", "No file name specified.");
            return;
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (int i = 0; i < ui->taskList->count(); ++i) {
                out << ui->taskList->item(i)->text() << "\n";
            }
            file.close();
            QMessageBox::information(this, "Save Tasks", "All tasks saved successfully to " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Could not save tasks to the specified file.");
        }
        return; // Exit after saving all tasks
    }

    if (reply == QMessageBox::No) {
        // Enable multiple task selection
        ui->taskList->setSelectionMode(QAbstractItemView::MultiSelection);

        // Prompt the user to select tasks
        QMessageBox::information(this, "Select Tasks", "Please select the tasks you want to save and press Save again.");
        return; // Exit here to let the user select tasks
    }
}

void MainWindow::handleLoadTasks() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Task File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) {
        QMessageBox::information(this, "Load Tasks", "No file selected.");
        return;
    }

    QFile file(fileName); // Open the selected file
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->taskList->clear(); // Clear existing tasks in the list
        while (!in.atEnd()) {
            QString task = in.readLine(); // Read each line
            ui->taskList->addItem(task); // Add to the list
        }
        file.close();
        QMessageBox::information(this, "Load Tasks", "Tasks loaded successfully from " + fileName);
    } else {
        QMessageBox::warning(this, "Error", "Could not open the selected file.");
    }
}

void MainWindow::handleClearAllTasks() {
    if (ui->taskList->count() == 0) {
        QMessageBox::information(this, "Clear All", "There are no tasks to clear.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Clear All Tasks",
        "Are you sure you want to clear all tasks?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        ui->taskList->clear(); // Clears all tasks from the list
        QMessageBox::information(this, "Clear All", "All tasks have been cleared.");
    }
}

