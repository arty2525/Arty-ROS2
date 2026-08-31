from mobile_robot_diagnostics.runtime_monitor import TopicWatch


def test_topic_watch_starts_without_data():
    watch = TopicWatch(name="/joint_states", timeout_seconds=1.0)
    assert watch.age_seconds(1_000_000_000) is None
    assert not watch.is_fresh(1_000_000_000)


def test_topic_watch_freshness():
    watch = TopicWatch(name="/joint_states", timeout_seconds=1.0)
    watch.mark(1_000_000_000)
    assert watch.is_fresh(1_500_000_000)
    assert not watch.is_fresh(2_500_000_000)
