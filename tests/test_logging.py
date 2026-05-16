import os
import logging
from src.core.logging import setup_logging, logger

def test_logging_setup(tmp_path, monkeypatch):
    mock_log_dir = tmp_path / "log"
    monkeypatch.setattr("src.core.logging.LOG_DIR", str(mock_log_dir))
    
    logger.handlers.clear()
    
    test_logger = setup_logging()
    
    assert test_logger.level == logging.INFO
    assert len(test_logger.handlers) >= 1
    
    test_logger.info("Test log message")
    
    log_file_path = os.path.join(str(mock_log_dir), "app.log")
    assert os.path.exists(log_file_path)
    
    with open(log_file_path, "r") as f:
        log_contents = f.read()
        assert "Test log message" in log_contents
