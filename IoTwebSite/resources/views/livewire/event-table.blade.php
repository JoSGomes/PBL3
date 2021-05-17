<div>
    <div class="container">
        <div class="row">
            <table class="table table-hover table-bordeless">
                <thead>
                <tr>
                    <th scope="col">Horário</th>
                    <th scope="col">Evento</th>
                    <th scope="col">Descrição</th>
                </tr>
                </thead>
                <tbody>
                <tr >
                    @foreach ($events as $event)
                    <tr >
                         <td>{{$event->hour}}</td>
                         <td>{{$event->name}}</td>
                         <td>{{$event->description}}</td>
                    </tr>
                    @endforeach
                </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>
