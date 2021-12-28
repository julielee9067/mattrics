from django.db import models


class Therapist(models.Model):
    email = models.CharField(max_length=50, null=False, blank=False)
    password = models.CharField(max_length=100, null=False, blank=False)
    first_name = models.CharField(max_length=30, null=False, blank=False)
    last_name = models.CharField(max_length=30, null=False, blank=False)
    middle_name = models.CharField(max_length=30)
    last_activated = models.DateTimeField(auto_now_add=True, null=False, blank=False)


class Patient(models.Model):
    therapist = models.ForeignKey(Therapist, on_delete=models.CASCADE)
    first_name = models.CharField(max_length=30, null=False, blank=False)
    last_name = models.CharField(max_length=30, null=False, blank=False)
    middle_name = models.CharField(max_length=30)
    gmfcs_level = models.SmallIntegerField(null=False, blank=False)
    date_of_birth = models.DateField(null=False, blank=False)


class Rental(models.Model):
    patient = models.OneToOneField(Patient, on_delete=models.CASCADE)
    start_date = models.DateField(null=False, blank=False)
    end_date = models.DateField(null=False, blank=False)
    returned_date = models.DateField(null=True)


class PressureData(models.Model):
    pressure_pattern_image_path = models.CharField(
        max_length=200, null=False, blank=False
    )


class MovementData(models.Model):
    movement_pattern_image_path = models.CharField(
        max_length=200, null=False, blank=False
    )


class BreathingData(models.Model):
    AHI = models.IntegerField()
    highest_rate = models.IntegerField()
    average_rate = models.IntegerField()
    low_rate_percentage = models.IntegerField()
    mid_rate_percentage = models.IntegerField()
    high_rate_percentage = models.IntegerField()


class DailyAnalyzedData(models.Model):
    patient = models.ForeignKey(Patient, on_delete=models.CASCADE)
    pressure = models.ForeignKey(PressureData, on_delete=models.DO_NOTHING)
    movement = models.ForeignKey(MovementData, on_delete=models.DO_NOTHING)
    breathing = models.ForeignKey(BreathingData, on_delete=models.DO_NOTHING)
    time_to_fall_asleep = models.IntegerField()
    sleep_awakenings = models.IntegerField()
    time_recorded = models.DateField(null=False, blank=False, auto_now_add=True)


class WeeklyAnalyzedData(models.Model):
    patient = models.ForeignKey(Patient, on_delete=models.CASCADE)
    pressure = models.ForeignKey(PressureData, on_delete=models.DO_NOTHING)
    movement = models.ForeignKey(MovementData, on_delete=models.DO_NOTHING)
    breathing = models.ForeignKey(BreathingData, on_delete=models.DO_NOTHING)
    time_recorded = models.DateField(null=False, blank=False, auto_now_add=True)


class RawData(models.Model):
    patient = models.ForeignKey(Patient, on_delete=models.CASCADE)
    pressure_map = models.JSONField(null=False, blank=False)
    time_recorded = models.DateTimeField(null=False, blank=False, auto_now_add=True)
