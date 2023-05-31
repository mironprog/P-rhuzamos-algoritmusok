import java.time.Duration;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

class Checkpoint implements Runnable {
    private final BlockingQueue<AtomicInteger> queue;
    private final AtomicBoolean isOn;

    private static final int MIN_SCORE = 10;
    private static final int MAX_SCORE = 100;

    public Checkpoint(BlockingQueue<AtomicInteger> queue, AtomicBoolean isOn) {
        this.queue = queue;
        this.isOn = isOn;
    }

    @Override
    public void run() {
        while(isOn.get()) {
            AtomicInteger awardedScore;
            try {
                awardedScore = queue.poll(2, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            if (awardedScore == null) continue;

            synchronized (awardedScore) {
                awardedScore.set(getRandomScore());
                awardedScore.notify();
            }
        }
    }

    private int getRandomScore() {
        return ThreadLocalRandom.current().nextInt(MIN_SCORE, MAX_SCORE + 1);
    }
}

class Player implements Runnable {
    private static final double MIN_TRAVEL_TIME_SEC = 0.5;
    private static final double MAX_TRAVEL_TIME_SEC = 2;
    private static final long ONE_SEC_IN_MILLISEC = 1000;

    private final int id;
    private final ConcurrentHashMap<Integer, Integer> scores;
    private final List<BlockingQueue<AtomicInteger>> checkpointQueues;
    private final AtomicInteger checkpointScore;
    private final AtomicBoolean isOn;
    private int currentCheckpointIdx = -1;

    public Player(
            int id,
            ConcurrentHashMap<Integer, Integer> scores,
            AtomicInteger checkpointScore,
            List<BlockingQueue<AtomicInteger>> checkpointQueues,
            AtomicBoolean isOn
    ) {
        this.id = id;
        this.scores = scores;
        this.checkpointScore = checkpointScore;
        this.checkpointQueues = checkpointQueues;
        this.isOn = isOn;
    }

    @Override
    public void run() {
        while (isOn.get()){
            final BlockingQueue<AtomicInteger> checkpoint = chooseCheckpoint();
            reachCheckpoint(checkpoint);

            while (isOn.get()) {
                try {
                    synchronized (checkpointScore) {
                        checkpointScore.wait(Duration.ofSeconds(3).toMillis());
                    }
                } catch (InterruptedException e) {
                    System.out.println(e.getMessage());
                }

                if (checkpointScore.get() == 0) continue;

                registerScore();
                break;
            }
        }
    }

    private BlockingQueue<AtomicInteger> chooseCheckpoint() {
        currentCheckpointIdx = ThreadLocalRandom.current().nextInt(0,checkpointQueues.size());
        return checkpointQueues.get(currentCheckpointIdx);
    }

    private void reachCheckpoint(BlockingQueue<AtomicInteger> checkpoint) {
        final double travelTime = ThreadLocalRandom.current().nextDouble(MIN_TRAVEL_TIME_SEC, MAX_TRAVEL_TIME_SEC);
        try {
            Thread.sleep(Double.valueOf(travelTime * ONE_SEC_IN_MILLISEC).longValue());
            checkpoint.put(checkpointScore);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    private void registerScore() {
        scores.compute(id, (id, score) -> score + checkpointScore.get());
        System.out.println(String.format("Player %d got %d points at checkpoint %d", id, checkpointScore.get(),
                currentCheckpointIdx));
        checkpointScore.set(0);
    }
}

public class FieldRace {
    private final static int PLAYER_COUNT = 5;
    private final static int CHECKPOINT_COUNT = 5;
    private final static int RACE_DURATION_SEC = 10;
    private final static int DISPLAY_SCORE_FREQUENCY_SEC = 1;

    private AtomicBoolean isOn = new AtomicBoolean(true);
    private ConcurrentHashMap<Integer, Integer> scores = new ConcurrentHashMap<>(PLAYER_COUNT);
    private AtomicInteger[] checkpointScores;
    private List<BlockingQueue<AtomicInteger>> checkpointQueues = Collections.synchronizedList(new ArrayList<>());

    private ExecutorService executor = Executors.newFixedThreadPool(PLAYER_COUNT + CHECKPOINT_COUNT + 1);

    private void displayScores() {
        final StringBuilder stringBuilder = new StringBuilder("Scores: [");

        scores.entrySet().stream().sorted(Collections.reverseOrder(Map.Entry.comparingByValue())).forEach((entry)
                -> stringBuilder.append(String.format("%s=%s, ", entry.getKey(), entry.getValue())));

        stringBuilder.delete(stringBuilder.length() - 2, stringBuilder.length());
        stringBuilder.append("]");
        System.out.println(stringBuilder);
    }

    public FieldRace() {
        for (int playerIdx = 0; playerIdx < PLAYER_COUNT; playerIdx++) {
            scores.put(playerIdx, 0);
        }

        checkpointScores = new AtomicInteger[PLAYER_COUNT];
        Arrays.fill(checkpointScores, new AtomicInteger(0));

        for (int checkpointIdx = 0; checkpointIdx < CHECKPOINT_COUNT; checkpointIdx++) {
            checkpointQueues.add(new LinkedBlockingQueue<>());
        }
    }

    public void start() throws InterruptedException {
        final Runnable displayScoresThread = () -> {
            while (isOn.get()) {
                displayScores();

                try {
                    Thread.sleep(Duration.ofSeconds(DISPLAY_SCORE_FREQUENCY_SEC).toMillis());
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        };
        executor.submit(displayScoresThread);

        for (int playerIdx = 0; playerIdx < PLAYER_COUNT; playerIdx++) {
            executor.submit(new Player(playerIdx, scores, checkpointScores[playerIdx], checkpointQueues, isOn));
        }

        for (int checkpointIdx = 0; checkpointIdx < CHECKPOINT_COUNT; checkpointIdx++) {
            executor.submit(new Checkpoint(checkpointQueues.get(checkpointIdx), isOn));
        }

        Thread.sleep(Duration.ofSeconds(RACE_DURATION_SEC).toMillis());

        isOn.set(false);
        executor.shutdown();
        executor.awaitTermination(3, TimeUnit.SECONDS);
        executor.shutdownNow();

        System.out.print("Final scores: ");
        displayScores();
    }


    public static void main(String[] args) throws InterruptedException {
        FieldRace race = new FieldRace();
        race.start();
    }
}