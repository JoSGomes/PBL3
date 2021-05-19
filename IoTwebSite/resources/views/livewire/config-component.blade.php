<div>
    <div class="subtitle">Configurações</div>
    <form wire:submit.prevent = "setConfig">
        <div class="container">
            @if ($sucessMessage)
            <div class="alert alert-dark alert-dismissible fade show" role="alert">                
                <strong>{{$sucessMessage}}</strong>
                <button wire:click = "resetMessage" type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>
            </div>
            @endif
            <div class="row">
                <div class="col align-self-start">
                    <label class="form-label" for="tempo">Ajustar o tempo de verificação de conexão</label>
                    <input wire:model="intervalConnection" class="form-control" type="text" name="tempoConnection" id="">
                </div>
                <div class="col align-self-end">
                    <select wire:model = "timeUnitConnection"  class="form-select" id="inputGroupSelect01">
                        <option value="1">Segundos</option>
                        <option value="60">Minutos</option>
                        <option value="3600">Horas</option>
                        </select>
                </div>
            </div>
            <div class="row">
                <div class="col align-self-start ">
                    <label class="form-label " for="tempo">Ajustar o tempo de acionamento do alarme</label>
                    <input wire:model="intervalAlarm" class="form-control" type="text" name="tempoAlarm" id="">
                </div>
                <div class="col align-self-end">
                    <select wire:model = "timeUnitAlarm" class="form-select" id="inputGroupSelect01">
                        <option value="1">Segundos</option>
                        <option value="60">Minutos</option>
                        <option value="3600">Horas</option>
                        </select>
                </div>
            </div>
            <div class="row position-relative pt-3">
                <div class="col-3">
                    <button
                        wire:loading.attr="disabled"
                        wire:target="setConfig" 
                        class="btn btn-outline-light me-md-2">
                        Salvar configurações
                    </button>
                </div>
                <div class="col">
                    <span
                    wire:loading 
                    wire:target="setConfig" 
                    class="spinner-border text-light pt-3" 
                    role="status">
                    </span>
                </div>
            </div>
            <div class="row">
                @error('intervalAlarm') <span class="error alert alert-danger">{{ $message }}</span> <br> @enderror
                @error('intervalConnection') <span class="error alert alert-danger">{{ $message }}</span> <br> @enderror
            </div>
        </div>
    </form>
</div>
