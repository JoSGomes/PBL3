<div>
    <div class="container">
        <div class="row">
            <div class="col-2">
                <input wire:model = 'day' type="number" class="number">
            </div>
            <div class="col-2">
                <select wire:model ='month' class="form-select form-select-sm" aria-label=".form-select-sm example">
                    <option selected>Open this select menu</option>
                    <option value="1">Janeiro</option>
                    <option value="2">Fevereiro</option>
                    <option value="3">Março</option>
                    <option value="4">Abril</option>
                    <option value="5">Maio</option>
                    <option value="6">Junho</option>
                    <option value="7">Julho</option>
                    <option value="8">Agosto</option>
                    <option value="9">Setembro</option>
                    <option value="10">Outubro</option>
                    <option value="11">Novembro</option>
                    <option value="12">Dezembro</option>
                  </select>
            </div>
            <div class="col-2">
                <button
                    wire:loading.attr="disabled"
                    wire:target="search" 
                    wire:click='search'
                    class="btn btn-outline-light me-md-2">
                    Pesquisar
                </button>
                <span
                    wire:loading 
                    wire:target="search" 
                    class="spinner-border text-light pt-3" 
                    role="status">
                </span>
            </div>
        </div>
    </div>
    <div class="container">
        <div class="row">
            @if($events)
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
            @endif
        </div>
    </div>
</div>
