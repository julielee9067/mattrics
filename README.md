## run

### run migration

```
python manage.py migrate
```

### run server

```bash
python manage.py runserver
```

## Pre-commit installation
### install
```
pip install pre-commit
```
```
pre-commit install
```
### run
- run against all files
  ```
  pre-commit run --all-files
  ```
- In other cases, running  ```git commit``` would automatically check the files

  ```
  git commit -m "{COMMIT MESSAGE}"
  # automatically check committed files
  
  git add {CHANGED FILES}
  
  # repeat
  git commit -m "{COMMIT MESSAGE}"
  ```
