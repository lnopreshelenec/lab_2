#include "notemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

NoteManager::NoteManager() {}

void NoteManager::sortNotes() {
    std::sort(notes.begin(), notes.end());
}

void NoteManager::addNote(const NOTE& note) {
    notes.append(note);
    sortNotes();
}

void NoteManager::addNoteAt(int index, const NOTE& note) {
    if (index >= 0 && index <= notes.size()) {
        notes.insert(index, note);
        sortNotes();
    }
}

void NoteManager::removeNote(int index) {
    if (index >= 0 && index < notes.size()) {
        notes.remove(index);
    }
}

void NoteManager::editNote(int index, const NOTE& newNote) {
    if (index >= 0 && index < notes.size()) {
        notes[index] = newNote;
        sortNotes();
    }
}

QVector<NOTE> NoteManager::findByLastName(const QString& lastName) const {
    QVector<NOTE> result;
    for (const auto& note : notes) {
        if (note.hasLastName(lastName)) {
            result.append(note);
        }
    }
    return result;
}

void NoteManager::saveToFile(const QString& filename) {
    std::ofstream file(filename.toStdString());
    if (file.is_open()) {
        file << notes.size() << std::endl;
        for (const auto& note : notes) {
            file << note << std::endl;
        }
        file.close();
    }
}

void NoteManager::loadFromFile(const QString& filename) {
    std::ifstream file(filename.toStdString());
    if (file.is_open()) {
        notes.clear();
        int count;
        file >> count;

        for (int i = 0; i < count; ++i) {
            NOTE note;
            file >> note;
            notes.append(note);
        }
        file.close();
        sortNotes();
    }
}

QVector<NOTE> NoteManager::getAllNotes() const {
    return notes;
}

NOTE NoteManager::getNote(int index) const {
    if (index >= 0 && index < notes.size()) {
        return notes[index];
    }
    return NOTE();
}

int NoteManager::getNoteCount() const {
    return notes.size();
}

void NoteManager::clear() {
    notes.clear();
}
