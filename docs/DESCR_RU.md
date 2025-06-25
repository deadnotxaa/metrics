# Библиотека для сбора и сохранения метрик

## Описание проекта

В рамках данного проекта была разработана производительная библиотека на C++ для сбора, обработки и сохранения метрик в различных форматах. Библиотека позволяет отслеживать именованные события, происходящие в произвольные моменты времени, с соответствующими значениями заданных типов данных.

## Архитектура решения

Архитектура библиотеки разработана с акцентом на гибкость, расширяемость и высокую производительность. Основные компоненты системы:

### Модульная структура

Библиотека построена на принципах SOLID, где каждый компонент имеет единую ответственность и слабую связанность с другими компонентами:

1. **Интерфейс для метрик (`AbstractMetric`)** — абстрактный класс, определяющий базовый функционал для всех типов метрик.

2. **Система калькуляторов (`MetricCalculatorInterface`)** — интерфейс для различных стратегий вычисления метрик:
   - `MetricSumCalculator` — суммирование значений
   - `MetricAverageCalculator` — вычисление среднего значения

3. **Подсистема записи (`MetricsWriterInterface`)** — интерфейс для различных способов сохранения метрик:
   - `MetricsFileWriter` — запись метрик в текстовый файл
   - `MetricsPrometheusWriter` — экспорт метрик для Prometheus
   - Возможность легкого добавления новых видов записи (например, в базу данных)

4. **Процессор метрик (`MetricsProcessor`)** — центральный компонент, управляющий сбором и обработкой метрик

### Lock-free архитектура

Одно из ключевых преимуществ решения — неблокирующая архитектура:

- Применена lock-free реализация для всех критических операций
- Использование конкурентных коллекций данных для хранения метрик (конкретно concurrentqueue)
- Распараллеливание обработки метрик без взаимных блокировок
- Thread pool для эффективного использования вычислительных ресурсов

## Расширенная функциональность

### Интеграция с системами мониторинга

В дополнение к базовым требованиям, была реализована полная интеграция с экосистемой мониторинга:

- **Prometheus экспортер** — специализированный writer для экспорта метрик в Prometheus
- **Grafana дашборды** — визуализация собираемых метрик (из Prometheus)
- **Контейнеризация** — Docker-образ с настроенным окружением

### Высокая производительность

Библиотека оптимизирована для высоконагруженных сценариев:

- Минимальное влияние на производительность основного приложения
- Асинхронная запись метрик
- Автоматический сброс накопленных значений после записи

## Технические детали реализации

### Использование сторонних библиотек

Для обеспечения lock-free функциональности и эффективного параллелизма использовались следующие библиотеки:

- Thread pool библиотека для эффективного управления потоками
- Lock-free структуры данных для неблокирующего доступа
- Prometheus C++ клиент для интеграции с системой мониторинга

### Гибкость типов метрик

Система поддерживает различные типы метрик без необходимости изменения основного кода:

- Числовые метрики (целочисленные, с плавающей точкой)
- Возможность добавления пользовательских типов через шаблоны

## Примеры использования (с Prometheus и Grafana)

```cpp
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution doubleDistribution(0.0, 2.0);
    std::uniform_int_distribution msDistribution(1, 50);

    // Create Metrics (also it supports not only double and int, but also any other types)
    metrics_task::AbstractMetric* metric =
        new metrics_task::Metric<double>("CPU Burst", new metrics_task::MetricAverageCalculator<double>());

    metrics_task::AbstractMetric* metric2 =
        new metrics_task::Metric<int>("HTTP Requests RPS", new metrics_task::MetricSumCalculator<int>());

    // Create Processor (he will process all metrics with specified interval and then send them to the specific writer)
    metrics_task::MetricsProcessor processor(
        {metric, metric2},
        new metrics_task::MetricsPrometheusWriter({"CPU Burst", "HTTP Requests RPS"}), // will be transformed to cpu_burst and http_request_rps in Prometheus
        std::chrono::milliseconds(1000)
    );

    // Start our processing. It will process metrics every second
    processor.startProcessing();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::seconds(600);

    while (start < end) {
        start = std::chrono::system_clock::now();

        metric->countMetric(doubleDistribution(gen));
        metric2->countMetric(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(msDistribution(gen)));
    }

    // At the end we stop processing and writing metrics
    processor.stopProcessing();
```

Здесь конкретно пример с случайными значениями. Код проработает 10 минут, в графане после настройки Data Source будут отображаться графики с записанными значениями. 

## Запуск с Docker

Для удобства использования и тестирования предоставлен Docker-образ:

```bash
docker-compose up -d
```

После запуска:
- Приложение доступно на порту 8080
- Prometheus доступен на порту 9090
- Grafana доступна на порту 3000 с предварительно настроенными дашбордами (НО необходимо вручную настроить Data Source: Prometheus, вписать тольок URL: http://prometheus:9090). Также можно будет в самих дашбордах отредактировать min interval для query в зависимости от частоты записи метрик. 

## Заключение

Разработанная библиотека представляет собой производительное, гибкое и расширяемое решение для сбора метрик. Благодаря модульной архитектуре, соблюдению принципов SOLID и lock-free реализации, библиотека может быть легко адаптирована под различные сценарии использования и интегрирована в существующие системы мониторинга.
