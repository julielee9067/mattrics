CREATE TABLE core.Therapist(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    firstName varchar(30) NOT NULL,
    lastName varchar(30) NOT NULL,
    email varchar(30) NOT NULL,
    password varchar(30) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE core.Patient(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    therapistId varchar(50) NOT NULL,
    firstName varchar(30) NOT NULL,
    lastName varchar(30) NOT NULL,
    email varchar(30) NOT NULL,
    gmfcsLevel int NOT NULL,
    dateOfBirth DATE NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (therapistId) REFERENCES core.Therapist(_id)
);

CREATE TABLE core.Rental(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId varchar(50) NOT NULL,
    startDate DATE NOT NULL,
    endDate DATE NOT NULL,
    returnedDate DATE NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES core.Patient(_id)
);

CREATE TABLE core.Pressure(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    path varchar(200) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE core.Movement(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    path varchar(200) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE core.Breathing(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    AHI int,
    highestRate int,
    averageRate int,
    lowRatePercentage int,
    midRatePercentage int,
    highRatePercentage int,
    PRIMARY KEY (_id)
);

CREATE TABLE core.DailyData(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId varchar(50) NOT NULL,
    pressureId varchar(50) NOT NULL,
    movementId varchar(50) NOT NULL,
    breathingId varchar(50) NOT NULL,
    timeToFallAsleep int,
    sleepAwakening int,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES core.Patient(_id),
    FOREIGN KEY (pressureId) REFERENCES core.Pressure(_id),
    FOREIGN KEY (movementId) REFERENCES core.Movement(_id),
    FOREIGN KEY (breathingId) REFERENCES core.Breathing(_id)
);

CREATE TABLE core.WeeklyData(
    _id varchar(50) NOT NULL,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId varchar(50) NOT NULL,
    pressureId varchar(50) NOT NULL,
    movementId varchar(50) NOT NULL,
    breathingId varchar(50) NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES core.Patient(_id),
    FOREIGN KEY (pressureId) REFERENCES core.Pressure(_id),
    FOREIGN KEY (movementId) REFERENCES core.Movement(_id),
    FOREIGN KEY (breathingId) REFERENCES core.Breathing(_id)
);
